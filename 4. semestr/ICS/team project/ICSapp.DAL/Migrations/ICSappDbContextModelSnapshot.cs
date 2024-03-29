﻿// <auto-generated />
using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;

namespace ICSapp.DAL.Migrations
{
    [DbContext(typeof(AppDbContext))]
    partial class ICSappDbContextModelSnapshot : ModelSnapshot
    {
        protected override void BuildModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("ProductVersion", "2.2.4-servicing-10062")
                .HasAnnotation("Relational:MaxIdentifierLength", 128)
                .HasAnnotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn);

            modelBuilder.Entity("ICSapp.DAL.Entities.Comment", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd();

                    b.Property<Guid?>("AuthorId");

                    b.Property<string>("MessageContent");

                    b.Property<Guid?>("PostId");

                    b.Property<DateTime>("Time");

                    b.HasKey("Id");

                    b.HasIndex("AuthorId");

                    b.HasIndex("PostId");

                    b.ToTable("Comments");
                });

            modelBuilder.Entity("ICSapp.DAL.Entities.Post", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd();

                    b.Property<Guid?>("AuthorId");

                    b.Property<string>("MessageContent");

                    b.Property<Guid?>("TeamId");

                    b.Property<DateTime>("Time");

                    b.Property<string>("Title");

                    b.HasKey("Id");

                    b.HasIndex("AuthorId");

                    b.HasIndex("TeamId");

                    b.ToTable("Posts");
                });

            modelBuilder.Entity("ICSapp.DAL.Entities.Team", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd();

                    b.Property<string>("TeamName");

                    b.HasKey("Id");

                    b.ToTable("Teams");
                });

            modelBuilder.Entity("ICSapp.DAL.Entities.User", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd();

                    b.Property<bool>("Activity");

                    b.Property<string>("Description");

                    b.Property<string>("EmailAdress");

                    b.Property<string>("FirstName");

                    b.Property<DateTime>("LastActivity");

                    b.Property<string>("Password");

                    b.Property<byte[]>("Picture");

                    b.Property<string>("Surname");

                    b.HasKey("Id");

                    b.ToTable("Users");
                });

            modelBuilder.Entity("ICSapp.DAL.Entities.UserTeam", b =>
                {
                    b.Property<Guid>("UserId");

                    b.Property<Guid?>("TeamId");

                    b.Property<Guid>("Id");

                    b.HasKey("UserId", "TeamId");

                    b.HasIndex("TeamId");

                    b.ToTable("UserTeam");
                });

            modelBuilder.Entity("ICSapp.DAL.Entities.Comment", b =>
                {
                    b.HasOne("ICSapp.DAL.Entities.User", "Author")
                        .WithMany()
                        .HasForeignKey("AuthorId");

                    b.HasOne("ICSapp.DAL.Entities.Post")
                        .WithMany("Contributions")
                        .HasForeignKey("PostId");
                });

            modelBuilder.Entity("ICSapp.DAL.Entities.Post", b =>
                {
                    b.HasOne("ICSapp.DAL.Entities.User", "Author")
                        .WithMany()
                        .HasForeignKey("AuthorId");

                    b.HasOne("ICSapp.DAL.Entities.Team")
                        .WithMany("Posts")
                        .HasForeignKey("TeamId");
                });

            modelBuilder.Entity("ICSapp.DAL.Entities.UserTeam", b =>
                {
                    b.HasOne("ICSapp.DAL.Entities.Team", "Team")
                        .WithMany("UserLinks")
                        .HasForeignKey("TeamId")
                        .OnDelete(DeleteBehavior.Cascade);

                    b.HasOne("ICSapp.DAL.Entities.User", "User")
                        .WithMany("TeamLinks")
                        .HasForeignKey("UserId")
                        .OnDelete(DeleteBehavior.Cascade);
                });
#pragma warning restore 612, 618
        }
    }
}
